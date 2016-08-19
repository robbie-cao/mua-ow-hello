include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_NAME:=function_key
#Version: 1.0-1
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_MAINTAINER:=JPH
# PKG_SOURCE_URL:=

define Package/function_key
    SECTION:=utils
	CATEGORY:=Utilities
	DEFAULT:=y
	TITLE:=Function Key -- catch function key input
	DEPENDS:=+libmraa
endef

#EXTRA_CFLAGS
EXTRA_LDFLAGS += -lmraa
#TARGET_CFLAGS+= -Wall

define Build/Prepare
	@echo "############## Build/Prepare"
	$(Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)  
endef
define Package/function_key/install
	@echo "############## Package/function_key/install"
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/function_key $(1)/usr/bin
endef

$(eval $(call BuildPackage,function_key))

