/*
 * Copyright 2014,2016 Vladimir Ermakov.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */
/**
 * @brief CUSTOM MSG plugin
 * @file custom_msg.cpp
 * @author Vladimir Ermakov <vooon341@gmail.com>
 *
 * @addtogroup plugin
 * @{
 */

#include "rcpputils/asserts.hpp"
#include "mavros/mavros_uas.hpp"
#include "mavros/plugin.hpp"
#include "mavros/plugin_filter.hpp"

#include "mavros_msgs/msg/custom_msg.hpp"

namespace mavros
{
namespace extra_plugins
{
using namespace std::placeholders;      // NOLINT

/**
 * @brief CUSTOM MSG plugin.
 * @plugin custom_msg
 */
class CustomMsgPlugin : public plugin::Plugin
{
public:
  explicit CustomMsgPlugin(plugin::UASPtr uas_)
  : Plugin(uas_, "custom_msg")
  {
    custom_msg_pub = node->create_publisher<mavros_msgs::msg::CustomMsg>("custom_msg", 10);
  }

  Subscriptions get_subscriptions() override
  {
    return {
      make_handler(&CustomMsgPlugin::handle_custom_msg),
    };
  }

private:
  rclcpp::Publisher<mavros_msgs::msg::CustomMsg>::SharedPtr custom_msg_pub;

  void handle_custom_msg(
    const mavlink::mavlink_message_t * msg [[maybe_unused]],
    mavlink::custom_msg::msg::CUSTOM_MSG & custom_msg,
    plugin::filter::SystemAndOk filter [[maybe_unused]])
  {
    auto cmsg = mavros_msgs::msg::CustomMsg();

    cmsg.timestamp = custom_msg.timestamp;
    cmsg.value = custom_msg.value;
    cmsg.custom_value = custom_msg.custom_value;

    custom_msg_pub->publish(cmsg);
  }
};
}       // namespace extra_plugins
}       // namespace mavros

#include <mavros/mavros_plugin_register_macro.hpp>  // NOLINT
MAVROS_PLUGIN_REGISTER(mavros::extra_plugins::CustomMsgPlugin)
