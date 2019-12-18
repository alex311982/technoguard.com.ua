[Smarty]
smarty_path_compile	= "compiled"
smarty_path_cache	= "cache"
smarty_force_compile	= false
EKV_SMARTY_USE_VERSION3 = 0; 0/1

[Core]
site_backend 		= "admin"
site_frontend		= "frontend"

default_frontend_module	= "base"
default_frontend_entity	= "page"
default_frontend_action	= "browse"

default_backend_module	= "news"
default_backend_entity	= "news"
default_backend_action	= "news_list"

backend_login_module	= "base"
backend_login_entity	= "user"
backend_login_action	= "login"

[Database]
default_field_delimiter = " "
default_id_field	= "id"

[Other]
BACKEND_DATETIME_FORMAT = "%d.%m.%Y %H:%M"
BACKEND_DATE_FORMAT = "%d.%m.%Y"
FRONTEND_DATE_FORMAT = "%d.%m.%Y %H:%M"
FRONTEND_DATE_FORMAT_PHP = "d F Y H:i:s"
DATE_FORMAT_PLAIN		= "d.m.Y H:i"
page_extension		= ".html"
DEFAULT_SELECT_VAL = 0
DEFAULT_SELECT_NAME = "--обрати--"


