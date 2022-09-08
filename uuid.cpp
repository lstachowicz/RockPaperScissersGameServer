//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "uuid.hpp"

using namespace RPSGame;

Uuid::Uuid(bool generate)
{
    if (generate)
    {
        ::uuid_generate(m_id);
    }
    else
    {
        ::uuid_clear(m_id);
    }
}

Uuid::Uuid(const std::string& hash)
{
    if (hash.empty() || ::uuid_parse(hash.c_str(), m_id) == -1)
    {
        ::uuid_generate(m_id);
    }
}

std::string Uuid::toString() const
{
    std::string result(36, '\0');
    ::uuid_unparse(m_id, result.data());
    return result;
}

Uuid& Uuid::operator=(const Uuid& uuid)
{
    if (this == &uuid)
    {
        return *this;
    }
    
    ::uuid_copy(this->m_id, uuid.m_id);
    return *this;
}

