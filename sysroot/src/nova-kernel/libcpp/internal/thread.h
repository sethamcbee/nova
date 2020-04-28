/**
 * @file thread.h
 * @author Seth McBee
 * @date 2020-4-26
 */

#pragma once

#include <stddef.h>

#include <utility>

namespace std
{

class thread
{
public:

    using id = size_t;

    id get_id() const noexcept
    {
        return id_;
    }

private:

    id id_;
};

} // namespace std
