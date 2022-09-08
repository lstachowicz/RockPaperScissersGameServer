//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include <string>
#include <uuid/uuid.h>

namespace RPSGame
{

class Uuid
{
public:
    Uuid(bool generate = true);
    Uuid(const std::string& hash);
    Uuid(const Uuid& uuid)
    {
        ::uuid_copy(m_id, uuid.m_id);
    }
    virtual ~Uuid() = default;

    Uuid& operator=(const Uuid& Uuid);
    
    inline const ::uuid_t& id() const { return m_id; }
    inline bool isNull() const { return ::uuid_is_null(m_id) == 1; }
    
    std::string toString() const;

private:
    ::uuid_t m_id;
};

inline bool operator==(const Uuid& lhs, const Uuid& rhs) { return ::uuid_compare(lhs.id(), rhs.id()) == 0; }
inline bool operator< (const Uuid& lhs, const Uuid& rhs) { return ::uuid_compare(lhs.id(), rhs.id()) == -1; }

}
