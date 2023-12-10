#ifndef LEPUS_SYSTEM_BASEAPP
#define LEPUS_SYSTEM_BASEAPP

namespace lepus
{
    namespace system
    {
        /// @brief A base app class that takes an init callback and immediately returns 0 when ran.
        class BaseApp
        {
            protected:
            /// @brief Has the app run the init function yet?
            bool m_Initialised;

            public:
            BaseApp()
            {
                m_Initialised = false;
            }

            BaseApp(void initCallback())
            {
                m_Initialised = false;
                Init(initCallback);
            }

            virtual inline void Init(void initCallback())
            {
                initCallback();
            }

            virtual inline int Run()
            {
                return 0;
            }
        };
    }
}

#endif