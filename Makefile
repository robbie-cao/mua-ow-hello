include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_NAME:=mhello
PKG_VERSION:=0.0.1
PKG_RELEASE:=1

define Package/mhello
    SECTION:=utils
	CATEGORY:=Utilities
	DEFAULT:=y
	TITLE:=Hello MUA - practice on openwrt
	DEPENDS:=+libmraa +libubus +libubox
	#DEPENDS:=+libmraa +libubus +libubox +libblobmsg_json
endef

EXTRA_LDFLAGS += -lmraa -lubus -lubox
#TARGET_CFLAGS+= -Wall

define Build/Prepare
	@echo "############## Build/Prepare"
	$(Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)  
endef
define Package/mhello/install
	@echo "############## Package/mhello/install"
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mkey $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/userver $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uclient $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mkeyd $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ks $(1)/usr/bin
endef

$(eval $(call BuildPackage,mhello))

