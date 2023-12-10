#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS
#define L3D_GRAPHICSENGINE_GRAPHICSAPI_BASEBINDINGS

#include "Uniforms.h"

namespace lepus
{
    namespace gfx
    {
        /// @brief An API-agnostic wrapper around a generic uniform.
        /// @tparam TUniformHandle API-specific uniform handle type
        /// @tparam TUniformValue Datatype held by this uniform (usually a POD type).
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

            /// @brief Sets the value of the uniform.
            /// @remarks Careful when passing data by reference/pointer - e.g. arrays. This class does NOT copy the data, and if the passed array has gone out of scope, the data will not be uploaded to the GPU!
            /// @param value 
            inline void Value(TUniformValue value)
            {
                m_Value = value;
                m_Dirty = true;
            }

            inline const TUniformValue& Value() const
            {
                return m_Value;
            }

            /// @brief Has the uniform been invalidated since it's last been applied?
            /// @return True if uniform has been invalidated and pending update in shader, false if no changes to data
            inline bool IsDirty() { return m_Dirty; }

            virtual UniformType Type() = 0;

            virtual void Release()
            {

            }

            virtual ~UniformBinding()
            {
                Release();
            }
        };
    }
}

#endif