#include "user.h"
user::user()
{

}

user& user::operator=(const user &u)
{
    id=u.getID();
    name=u.getname();
    t=u.getT();
    return *this;
}
