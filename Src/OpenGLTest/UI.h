
class UserInterfaceImp;
class UserInterface
{
public:
    UserInterface();
    ~UserInterface();

    bool Init();
    void Run();
    void Cleanup();

private:
    UserInterfaceImp* m_imp;
};
