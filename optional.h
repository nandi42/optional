#ifndef OPT_H
#define OPT_H

#include <type_traits>
#include <stdexcept>
#include <utility>

namespace opt
{

template<typename type>
class optional
{
    bool engaged;
    alignas(type) unsigned char storage[sizeof(type)];
    //alignas - the optional data will be aligned the same as the type it should be
    //unsigned char - universal container, can contain anything
    //with size of the type size array

public:

    optional() : engaged(false) {} //if we constructed it without value then it won't be engaged

    optional(const type& value) : engaged(true) {new (storage) type(value);}//if we gave it a value then it creates the value at storage space

    optional(type&& value) : engaged(true) {new (storage) type(std::move(value));}//if we gave it a value then it creates the value at storage space, for rvalues

    optional(const optional& other) : engaged(other.engaged)
    {
        if (engaged)
        {
            new (storage) type(other.value());
        }
    }//if we want to copy it fro another optional

    //we also need this for rvalues - for more, look up how rvalues and lvalues
    optional(optional&& other) noexcept(std::is_nothrow_move_constructible<type>::value) : engaged(other.engaged) //noexcept - there will be no exceptions thrown, except when the type throws
    //this improves performance, because instead of copying, we just move it
    {
        if (engaged)
        {
            new (storage) type(std::move(other.value()));
            other.reset();
        }
    }

    optional& operator=(const optional& other)
    {
        if (this != &other) //this object can't be on the same address as the other
        {
            if (other.engaged)
            {
                emplace(other.value());
            }
            else reset();//if not engaged, we just have to reset ours
        }
        return *this;
    }

    optional& operator=(optional&& other)//same for rvalues
    {
        if (this != &other) //this object can't be on the same address as the other
        {
            if (other.engaged)
            {
                emplace(std::move(other.value()));
            }
            else reset();//if not engaged, we just have to reset ours
        }
        return *this;
    }

    explicit operator bool() const
    {
        return engaged;
    }

    type& value()
    {
        if (engaged) return *reinterpret_cast<type*>(storage);
        else throw std::runtime_error("Optional doesn't contain value");
    }//asks for the value at storage

    const type& value() const {
        if (engaged) return *reinterpret_cast<const type*>(storage);
        else throw std::runtime_error("Optional doesn't contain value");
    }//for the constant

    //we need a reset - for any time we want to reset the value - we cannot erase it that easily, like a simple type
    void reset()
    {
        if(engaged)
        {
            value().~type(); //uses the reset of the type
            engaged = false;
        }
    }

    void emplace(const type& value) {
        reset();
        new (storage) type(value);
        engaged = true;
    }

    void emplace(type&& value)
    {
        reset();
        new (storage) type(std::move(value));
        engaged = true;
    }

    optional& operator=(const type& value)
    {
        emplace(value);
        return *this;
    }

    optional& operator=(type&& value)
    {
        emplace(std::move(value));
        return *this;
    }

    ~optional()
    {
        reset();
    }//the end reset
};

}

#endif // OPT_H
