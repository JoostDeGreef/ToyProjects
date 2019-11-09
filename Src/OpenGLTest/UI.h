#pragma once

class UserInterfaceImp;
class UserInterface
{
public:
    UserInterface(Data && data);
    ~UserInterface();

    bool Init();
    void Run();
    void Cleanup();

private:
    UserInterfaceImp* m_imp;
};
