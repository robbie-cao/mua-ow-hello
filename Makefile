include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=mhello
PKG_VERSION:=0.0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

define Package/mhello
    SECTION:=utils
	CATEGORY:=Utilities
	DEFAULT:=y
	TITLE:=Hello MUA - practice on openwrt
	DEPENDS:=+libmraa +libupm-pn532 +libubus +libubox
	#DEPENDS:=+libmraa +libubus +libubox +libblobmsg_json
endef

EXTRA_LDFLAGS += -lmraa -lupm-pn532 -lubus -lubox
#TARGET_CFLAGS+= -Wall

define Build/Prepare
	@echo "############## Build/Prepare"
	$(Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef
define Package/mhello/install
	@echo "############## Package/mhello/install"
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/function_key $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/userver $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uclient $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mkeyd $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ks $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pn532 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pn532-writeurl $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pn532-drv $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/caller $(1)/usr/bin
endef

$(eval $(call BuildPackage,mhello))

