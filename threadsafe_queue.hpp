// Listing 4.5 corrected and modernized by Craig Scratchley

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    mutable std::condition_variable empty_cond;

    void popping() {
       data_queue.pop();
       if (data_queue.empty())
          empty_cond.notify_all();
    }

public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard lk{other.mut};
        data_queue=other.data_queue;
    }

    void push(T new_value)
    {
        std::lock_guard lk{mut};
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void drain() const
    {
        std::unique_lock lk{mut};
        empty_cond.wait(lk, [this]{return data_queue.empty();}  );
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock lk{mut};
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        popping();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock lk{mut};
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        auto res{std::make_shared<T>(data_queue.front())};
        popping();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard lk{mut};
        if(data_queue.empty)
            return false;
        value=data_queue.front();
        popping();
        return true; // Added by Craig
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard lk{mut};
        if(data_queue.empty())
            return std::shared_ptr<T>();
        auto res{std::make_shared<T>(data_queue.front())};
        popping();
        return res;
    }

    bool empty() const
    {
        std::lock_guard lk{mut};
        return data_queue.empty();
    }
};

//int main()
//{}
