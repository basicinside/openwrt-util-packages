include $(TOPDIR)/rules.mk


PKG_NAME:=channel_switcher
# PkG_VERSION has to be long unsigned int
PKG_VERSION:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/channel_switcher
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=channel_switcher
  MAINTAINER:=Robin Kuck <robin@basicinside.de>
  DEPENDS:= +libnl-tiny
endef

define Package/channel_switcher/description
  This package contains a utility to control the
  power settings for a USB hub and resetting USB
  devices. It detect port states and turns power on and off.
endef



define Build/Prepare
		mkdir -p $(PKG_BUILD_DIR)
		$(CP) src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += $(FPIC)

define Build/Compile
		$(TARGET_CC) $(TARGET_CFLAGS) -I$(PKG_BUILD_DIR) \
			-o $(PKG_BUILD_DIR)/channel_switcher \
			-I$(STAGING_DIR)/usr/include/libnl-tiny -lnl-tiny \
			$(PKG_BUILD_DIR)/channel_switcher.c -DDEBUG
endef

define Package/channel_switcher/install
		$(INSTALL_DIR) $(1)/bin
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/channel_switcher $(1)/bin/
		$(INSTALL_DIR) $(1)/etc/channelswitch
		$(INSTALL_BIN) ./files/channel2 $(1)/etc/channelswitch/
		$(INSTALL_BIN) ./files/channel5 $(1)/etc/channelswitch/
endef

$(eval $(call BuildPackage,channel_switcher))
