#include "bgfx_compute.sh"

BUFFER_WO(m_VBO,    vec2, 0);
BUFFER_RO(m_Buffer, vec2, 1);

NUM_THREADS(1, 1, 1)
void main()
{
    m_VBO[0] = m_Buffer[0];
    m_VBO[1] = m_Buffer[1];
    m_VBO[2] = m_Buffer[2];
    m_VBO[3] = m_Buffer[3];
}
