//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#pragma once

#include <set>

template <class T>
class InactiveReciver
{
public:
    virtual void inactive(const std::set<T>& inactive) = 0;
};
