#include "ProcessHandler.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <sys/mman.h>

ProcessHandler::ProcessHandler(OutputObserverPtr& out)
    : outputObserver_(out)
{

}
ProcessHandler::ProcessHandler(ErrorObserverPtr& err)
    : errorObserver_(err)
{

}

ProcessHandler::ProcessHandler(OutputObserverPtr& out, ErrorObserverPtr& err)
    : outputObserver_(out)
    , errorObserver_(err)
{

}

ProcessHandler::~ProcessHandler() {
    if(nonBlockingThread_) {
        nonBlockingThread_->join();
    }
}

int ProcessHandler::execute(std::string command) {
    int stdout_fds[2];
    int unused = pipe(stdout_fds);

    int stderr_fds[2];
    unused = pipe(stderr_fds);
    if(unused) {}
    int* running = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    const pid_t pid = fork();
    if (!pid) {
        close(stdout_fds[0]);
        dup2(stdout_fds[1], STDOUT_FILENO);
        close(stdout_fds[1]);

        close(stderr_fds[0]);
        dup2(stderr_fds[1], STDERR_FILENO);
        close(stderr_fds[1]);

        *running = 1;
        unused = system(command.c_str());
        *running = 0;
        exit(0);
    }

    const int buf_size = 4096;

    std::thread outThread([&](){
        close(stdout_fds[1]);
        char buffer[buf_size];
        do {
            const ssize_t r = read(stdout_fds[0], buffer, buf_size);
            if (r > 0) {
                if(outputObserver_) (*outputObserver_)(std::string(buffer, r));
            }
        } while ((errno == EAGAIN || errno == EINTR || errno == 0) && *running);
        close(stdout_fds[0]);
    });


    std::thread errThread([&](){
        char buffer[buf_size];
        close(stderr_fds[1]);
        do {
            const ssize_t r = read(stderr_fds[0], buffer, buf_size);
            if (r > 0) {
                if(errorObserver_) (*errorObserver_)(std::string(buffer, r));
            }
        } while ((errno == EAGAIN || errno == EINTR || errno == 0) && *running);
        close(stderr_fds[0]);
    });

    outThread.join();
    errThread.join();

    int r, status;
    do {
        r = waitpid(pid, &status, 0);
    } while (r == -1 && errno == EINTR);

    munmap(running, sizeof(int));

    if(outputObserver_) outputObserver_->executionDone();
    return status;
}

void ProcessHandler::executeNonBlocking(std::string command) {
    nonBlockingThread_ = std::make_unique<std::thread>(&ProcessHandler::execute, this, command);
}
