#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS
#define L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS

#include "Uniforms.h"

namespace lepus
{
    namespace gfx
    {
        template<typename TUniformHandle = void*>
        class UniformBinding
        {
            private:
            size_t m_Size = 0;
            void* m_Value = nullptr;
            bool m_Dirty = false;

            public:
            TUniformHandle Location;

            UniformBinding(TUniformHandle location)
            {
                Location = location;
            }

            template<typename TUniformValue>
            inline void Value(TUniformValue value)
            {
                m_Value = reinterpret_cast<void*>(value);
                m_Dirty = true;
            }

            template<typename TUniformValue>
            inline TUniformValue Value()
            {
                return reinterpret_cast<TUniformValue>(m_Value);
            }

            inline bool IsDirty() { return m_Dirty; }

            virtual UniformType Type() = 0;

            virtual void Release()
            {

            }
        };
    }
}

#endif