include $(TOPDIR)/rules.mk


PKG_NAME:=usb-hubpower
# PkG_VERSION has to be long unsigned int
PKG_VERSION:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/usb-hubpower
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=usb-hubpower
  MAINtAINER:=Robin Kuck <robin@basicinside.de>
endef

define Package/usb-hubpower/description
  This package contains a utility to control the 
  power settings for a USB hub. It detect port states
  and turns power on and off.
endef



define Build/Prepare
		mkdir -p $(PKG_BUILD_DIR)
		$(CP) src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += $(FPIC)

define Build/Compile
		$(TARGET_CC) $(TARGET_CFLAGS) -I$(PKG_BUILD_DIR) \
			-o $(PKG_BUILD_DIR)/usb-hubpower \
			$(PKG_BUILD_DIR)/usb-hubpower.c
endef

define Package/usb-hubpower/install
		$(INSTALL_DIR) $(1)/bin
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb-hubpower $(1)/bin/
endef

$(eval $(call BuildPackage,usb-hubpower))