#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}
MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
    MyPrimaryGenerator *generator1 = new MyPrimaryGenerator();
    SetUserAction(generator1);
}
