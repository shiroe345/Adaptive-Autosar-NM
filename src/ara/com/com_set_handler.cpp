#include "com_set_handler.hpp"

map<NetworkRequestedState *,
 ara::core::Vector<std::function<ara::core::Future<NetworkRequestedState::FieldType>(const NetworkRequestedState::FieldType& data    )>>> handlers{};
