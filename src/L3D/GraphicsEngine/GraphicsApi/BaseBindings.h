#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS
#define L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS

#include "Uniforms.h"

namespace lepus
{
    namespace gfx
    {
        template<typename TUniformHandle, typename TUniformValue = void*>
        class UniformBinding
        {
            private:
            size_t m_Size = 0;
            TUniformValue m_Value;
            bool m_Dirty = false;
            protected:
            TUniformHandle m_Location;

            public:

            UniformBinding(TUniformHandle location)
            {
                memcpy(&m_Location, &location, sizeof(TUniformHandle));
            }

            virtual const TUniformHandle& Location() const = 0;

            inline void Value(TUniformValue value)
            {
                m_Value = value;
                m_Dirty = true;
            }

            inline const TUniformValue& Value() const
            {
                return m_Value;
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