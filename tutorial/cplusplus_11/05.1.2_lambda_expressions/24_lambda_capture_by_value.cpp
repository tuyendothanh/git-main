/* Header     : core feature
   Signature  : [ captures ] (params) specifiers exception attr -> ret { body }
                [ captures ] (params) -> ret { body }
                [ captures ] (params) { body }
                [ captures ] { body }

   Description:
       captures  : [] nothing
                   [=] all by copy and *this by reference (if any)
                   [&] all by reference and *this by reference (if any)
                   [this] *this by reference
                   [a, &b] a by copy, b by reference
       specifiers: mutable   - can modify all captures by copy and call their
                               non-const member functions
                   constexpr - default, obey constexpr functions
       exception : throw
                   noexcept
       attr      : [[attr]] [[attr1, attr2(args)]] [[namespace::attr(args)]] alignas_specifier
                   ex: [[noreturn]] void f() { throw "error" ; }
                   [[carries_dependency]]
                   [[deprecated]], [[deprecated("reason")]]
                   [[fallthrough]]
                   [[nodiscard]]
                   [[maybe_unused]], etc...
       ret       : default void or explicit, e.g. auto&, int&, char*, SomeClass
       body      : function body

   -----------------------------------------------------------------------------
   http://thispointer.com/c11-lambda-how-to-capture-local-variables-inside-lambda/

   How to capture local variables by value from outer scope in lambda expr.
   The variables specified in capture list will be copied inside lambda
   by value. Inside lambda they can be accessed but can not be changed,
   because they are const. To modify them we need to add mutable keyword.

   auto func = [msg, counter] () mutable { };

 */

#include <iostream>
#include <string>

using namespace std;

int main()
{
    std::string message = "Hello";
    int counter = 10;

    /* Capturing local variables by value */
    auto func = [message, counter] () mutable {
        cout << "message = " << message << '\n';
        cout << "counter = " << counter << '\n';

        message = "Temp";
        counter = 20;

        cout << "message = " << message << '\n';
        cout << "counter = " << counter << '\n';
    };

    func();


    cout << "message = " << message << '\n';
    cout << "counter = " << counter << '\n';

    return 0;
} // main