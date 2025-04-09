#include "bgfx_compute.sh"

BUFFER_WO(m_VBO,    vec4, 0);
BUFFER_WO(m_IBO,    uint, 1);
BUFFER_RO(m_Buffer, float, 2);

NUM_THREADS(1, 1, 1)
void main()
{
    m_VBO[0] = vec4(m_Buffer[0], m_Buffer[1], m_Buffer[2], m_Buffer[3]);
    m_VBO[1] = vec4(m_Buffer[4], m_Buffer[5], m_Buffer[6], m_Buffer[7]);

    m_IBO[0] = 0;
    m_IBO[1] = 3;
    m_IBO[2] = 1;

    m_IBO[3] = 0;
    m_IBO[4] = 2;
    m_IBO[5] = 3;

    // m_VBO[2] = -0.25;
    // m_VBO[3] = 0.25;

    // m_VBO[4] = 0.5;
    // m_VBO[5] = -0.5;

    // m_VBO[6] = 0.5;
    // m_VBO[7] = 0.5;
}
