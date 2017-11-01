/* Header     : <thread>
   Signature  :           thread() noexcept;
                template<class Fn, class... Args>
                explicit  thread(Fn&& fn, Args&&... args);
                          thread(const thread&) = delete;
                          thread(thread&& x) noexcept;

   Description:
       - thread represents a single thread of execution
       - begins execution immediately upon construction, starting at the
         top-level function provided as a constructor argument
       - the return value is ignored, and if it terminates by throwing exception
         std::terminate is called
       - the top-level function may communicate the return valu or an exception
         to the caller via std::promise (or other ways - shared variables)
       - thread looses its state after join(), detach(), default ctor(), moved
       - no 2 thread object may represent the same thread

   See also : http://en.cppreference.com/w/cpp/thread/thread
   -----------------------------------------------------------------------------

   This tutorial is copied from:
   http://thispointer.com/c11-multithreading-part-3-carefully-pass-arguments-to-threads
   http://thispointer.com/c11-tutorial

   How to pass references to thread.
   As arguments are copied to new threads stack, so you pass the references
   in common way.
*/

#include <iostream>
#include <thread>


void new_thread(int const& val)
{
    int& y = const_cast<int&>(val);
    y++;
    std::cout << __FUNCTION__ << ", y = " << y << std::endl;
}


int main()
{
    int val = 9;
    std::cout << "before, val = " << val << std::endl;

    std::thread threadObj (new_thread, val);
    threadObj.join();

    std::cout << "after, val = " << val << std::endl;

    return 0;
}
