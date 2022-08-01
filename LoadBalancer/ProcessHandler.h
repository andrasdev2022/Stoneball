#ifndef __PROCESSHANDLER_H__
#define __PROCESSHANDLER_H__
#include <string>
#include <memory>
#include <thread>

struct OutputObserver {
    virtual void operator()(std::string out) = 0;
    virtual bool isAppReady() const noexcept = 0;
    virtual bool isStopped() const noexcept = 0;
    virtual int clientsNumber() const noexcept = 0;
    virtual void executionDone() noexcept = 0;
    virtual bool isExecutionDone() const noexcept = 0;
};
using OutputObserverPtr = std::shared_ptr<OutputObserver>;

struct ErrorObserver {
    virtual void operator()(std::string err) = 0;
};
using ErrorObserverPtr = std::shared_ptr<ErrorObserver>;

class ProcessHandler
{
public:
    ProcessHandler() = default;
    ~ProcessHandler();
    ProcessHandler(const ProcessHandler&) = delete;
    ProcessHandler(ProcessHandler&&) = delete;
    ProcessHandler& operator=(const ProcessHandler&) = delete;
    ProcessHandler& operator=(const ProcessHandler&&) = delete;

    ProcessHandler(OutputObserverPtr& out);
    ProcessHandler(ErrorObserverPtr& err);
    ProcessHandler(OutputObserverPtr& out, ErrorObserverPtr& err);
    int execute(std::string command);
    void executeNonBlocking(std::string command);

    const OutputObserverPtr& outputObserver() const {return outputObserver_;}

private:
    OutputObserverPtr outputObserver_;
    ErrorObserverPtr errorObserver_;
    std::unique_ptr<std::thread> nonBlockingThread_;
};

using ProcessHandlerUPtr = std::unique_ptr<ProcessHandler>;

#endif // __PROCESSHANDLER_H__
