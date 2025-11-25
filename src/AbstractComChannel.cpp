#include "AbstractComChannel.hh"

/*!
 * \brief Udostępnia mutex w trybie modyfikacji
 */
std::mutex& AbstractComChannel::UseGuard() {
    static std::mutex dummy;
    return dummy;
}
