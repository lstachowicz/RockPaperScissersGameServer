//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include <string>

template <class T>
class InterfaceId
{
public:
    explicit InterfaceId() = default;
    explicit InterfaceId(const T& id) : m_id(id) { }
    explicit InterfaceId(const std::string& id) : m_id(id) { }
    virtual ~InterfaceId() = default;
    
    inline const T& getId() const { return m_id; }
    
private:
    T m_id;
};
