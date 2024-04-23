/*
 * CrossPlatform.hpp
 *
 *  Created on: Jan 11, 2021
 *      Author: andrewrubinstein
 */

#ifndef CROSSPLATFORM_HPP_
#define CROSSPLATFORM_HPP_
#include <inttypes.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "poll_simple.hpp"


#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR '\\'
#define libreadline 0
#else
#define PATH_SEPARATOR '/'
#if defined(libreadline)
#define libreadline 1
#include <readline/readline.h>
#include <readline/history.h>
#endif
#endif
class CrossPlatform {
public:
	CrossPlatform() {}
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
static void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

#else

static void usleep(uint64_t usec){
	std::this_thread::sleep_for(std::chrono::microseconds(usec));
}
#endif
//Converts an ascal style file path with . as the path separator to a native filepath in windows/unix
static std::string convertFilePath(std::string fp)
{
	uint32_t c = 0;
    for(; c<fp.size(); c++)
    {
        if(fp[c] == '.')
            fp[c] = (PATH_SEPARATOR);
    }
    while(fp[--c] != '.' && c) {}
    fp[c] = fp[c]*(!c) + PATH_SEPARATOR*(c!=0);
    return fp;
}
#if libreadline == 0
static void readLine(std::string &line, std::string prompt)
{
    std::cout<<prompt;
    getLine(std::cin, line);
}
static void getLine(std::istream &ascal_cin, std::string &line)
{
    getline(ascal_cin, line);
}

typedef std::chrono::time_point<std::chrono::system_clock> time_point;
static inline time_point now()
{
    return std::chrono::system_clock::now();
}
static inline uint64_t time_since_ms(time_point time)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(now() - time).count() ;
}
class Unix_IPC {
    pid_t child_pid = 0;
	int fd_to_child[2] {0};
    int fd_from_child[2] {0};
    int fd_from_child_err[2] {0};
    bool make_pipe(int fd[2])
    {
        if (pipe(fd) == -1) {
            perror("pipe");
            return false;
        }
        return true;
    }
    int from_child_fd()
    {
        return fd_from_child[0];
    }
    int from_child_err_fd()
    {
        return fd_from_child_err[0];
    }
    int to_child_fd()
    {
        return fd_to_child[1];
    }
    public:
    Unix_IPC() {}
    Unix_IPC(std::string cmd)
    {
        std::vector<char*> argv;

        for(size_t i = 0, last = 0; i <= cmd.size(); i++)
        {
            if(cmd[i] == ' ' || cmd[i] == '\0')
            {
                argv.push_back(&cmd[last]);
                cmd[i] = '\0';
                last = i + 1;
            }
        }
        argv.push_back(nullptr);
        spawn(argv[0], argv.data());
    }
    Unix_IPC(std::string program, char* const* argv)
    {
        spawn(program, argv);
    }
    void swap(Unix_IPC& o)
    {
        pid_t child_pid = o.child_pid;
	    int fd_from_child[2] {o.fd_from_child[0], o.fd_from_child[1]};
	    int fd_from_child_err[2] {o.fd_from_child_err[0], o.fd_from_child_err[1]};
	    int fd_to_child[2] {o.fd_to_child[0], o.fd_to_child[1]};

        o.child_pid = this->child_pid;
        memcpy(o.fd_from_child, this->fd_from_child, sizeof(this->fd_from_child));
        memcpy(o.fd_from_child_err, this->fd_from_child_err, sizeof(this->fd_from_child_err));
        memcpy(o.fd_to_child, this->fd_to_child, sizeof(this->fd_to_child));
        this->child_pid = child_pid;
        memcpy(this->fd_from_child, fd_from_child, sizeof(fd_from_child));
        memcpy(this->fd_from_child_err, fd_from_child_err, sizeof(fd_from_child_err));
        memcpy(this->fd_to_child, fd_to_child, sizeof(fd_to_child));
    }
    Unix_IPC(const Unix_IPC&& o)
    {
        swap(*const_cast<Unix_IPC*>(&o));
    }
    int spawn(std::string& program, char* const* argv)
    {
        return spawn(program.c_str(), argv);
    }
    int spawn(const char* program, char* const* argv)
    {
        if(!make_pipe(fd_to_child))
            throw "Error creating pipe.";
        if(!make_pipe(fd_from_child))
            throw "Error creating pipe.";
        if(!make_pipe(fd_from_child_err))
            throw "Error creating pipe.";
        child_pid = fork();
        if(child_pid == 0)
        {
            ::close(fd_from_child[0]);
            ::close(fd_from_child_err[0]);
            ::close(fd_to_child[1]);
            dup2(fd_to_child[0], STDIN_FILENO);
            dup2(fd_from_child[1], STDOUT_FILENO);
            dup2(fd_from_child_err[1], STDERR_FILENO);
            if (fcntl(fd_from_child[1], F_GETFD) == -1) perror("STDOUT fd error");
            if (fcntl(fd_from_child_err[1], F_GETFD) == -1) perror("STDERR fd error");
            ::close(fd_to_child[0]);
            ::close(fd_from_child[1]);
            ::close(fd_from_child_err[1]);
            return execvp(program, argv);
        }
        ::close(fd_to_child[0]);
        ::close(fd_from_child[1]);
        ::close(fd_from_child_err[1]);
        return child_pid;
    }
    bool is_open()
    {
        return is_valid_fd(from_child_fd());
    }
    //expects empty string will fill as much as it can
    //without blocking greater than timeout
    //this is buggy don't use for now
    bool try_read(std::string& buf, long timeout)
    {
        buf.resize(0);
        const size_t buff_size = 4096;
        auto start = now();
        size_t index = 0;
        buf.reserve(buff_size);
        while(timeout > 0 && poll_fd(from_child_fd(), timeout))
        {
            const auto bytes = ::read(from_child_fd(), (void*) (buf.data() + index), buf.size() - index);
            if(bytes == -1)
                throw "Exception could not read.";
            if(bytes == 0)
                break;
            index += bytes;
            if((long) (buf.size() - index) < buff_size)
                buf.reserve(buf.size() + buff_size);
            timeout = timeout - time_since_ms(start);
            start = now();
        }
        return index > 0;
    }
    void finish_sending()
    {
        if(is_valid_fd(to_child_fd()))
            ::close(to_child_fd());
    }
    void read_err(std::string& result)
    {
        finish_sending();
        const size_t buff_size = 4096;
        char buf[buff_size];
        int bytes = 0;
        while((bytes = ::read(from_child_err_fd(), buf, buff_size)) > 0)
        {
            if(bytes < 0)
            {
                perror("exception reading std error in read err");
                throw "Error reading.";
            }
            result.append(buf, bytes);
        }
    }
    void read_all(std::string& result)
    {
        finish_sending();
        const size_t buff_size = 4096;
        char buf[buff_size];
        int bytes = 0;
        while((bytes = ::read(from_child_fd(), buf, buff_size)) > 0)
        {
            if(bytes < 0)
            {
                perror("exception in read all");
                throw "Error reading.";
            }
            result.append(buf, bytes);
        }
    }
    void read(std::string& buf)
    {
        buf.resize(0);
        while(buf.size() == 0) 
        {
            try_read(buf, 20);
        }
    }
    void write(const std::string& buf)
    {
        long index = 0;
        while(index < buf.size())
        {
            const long bytes = ::write(to_child_fd(), (void*) (index + buf.c_str()), buf.size());
            if(bytes == -1)
            {
                perror("writing to pipe");
                break;
            }
            index += bytes;
        }
    }
    int close()
    {
        if(!child_pid)
            return 0;
        int process_exit_code = 0;
        if(is_valid_fd(to_child_fd()))
            ::close(to_child_fd());
        if(is_valid_fd(from_child_fd()))
            ::close(from_child_fd());
        if(is_valid_fd(from_child_err_fd()))
            ::close(from_child_err_fd());
        waitpid(child_pid, &process_exit_code, 0);
        child_pid = 0;
        return process_exit_code;
    }
    ~Unix_IPC()
    {
        close();
    }
};
#else

static bool readLine(std::string &line, std::string prompt)
{
    char *readLineBuffer = nullptr;
    if(prompt.size())
        readLineBuffer = readline(prompt.c_str());
    else
        readLineBuffer = readline("");

    auto bufCleaner = std::make_unique<char*>(readLineBuffer);
    if(readLineBuffer)
    {
        line = readLineBuffer;
        if (line.size() > 0) {
          add_history(readLineBuffer);
        }
    }
    return readLineBuffer;
}
static void getLine(std::istream &ascal_cin, std::string &line)
{
    //if you get compiler errors you can not use readLine
    if(ascal_cin.rdbuf() != std::cin.rdbuf())
        getline(ascal_cin, line);
    else
        readLine(line, "");
}
#endif

};

#endif /* CROSSPLATFORM_HPP_ */
