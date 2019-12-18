[MODE]
;LIVE_VERSION = 0; 0 | 1
LIVE_VERSION = 1;

EKV_SMARTY_USE_VERSION3 = 1;

[Common]
site_domain		= "technoguard.com.ua" ; site domain e.g. mysite.com
SITE_HOST		= "technoguard.com.ua" ; site domain e.g. mysite.com
COOKIE_DOMAIN = ".technoguard.com.ua";
site_title		= "technoguard.com.ua" ; Frontend site name
site_title_delimiter	= " | ";
DISPLAY_ERRORS = 1;

[Locale]
; Smarty Date Format
DEFAULT_DATE_FORMAT	= "%d.%m.%Y"
default_time_format  = "%H:%M"   ; Smarty Time Format
; Smarty Date Picker Order: D[ate] M[onth] Y[ear]
date_picker_order	= "DMY"
; PHP Date Format
date_format		= "d.m.Y"
; Locale HTML Charset
site_html_charset	= "UTF-8"


[Database]
db_host			= "localhost"
db_base			= "technoguard" ; mysql database name
db_user			= "u_technoguar" ; mysql database user
db_pass			= "X7JOfJTF" ; mysql database password
db_port			= ""
db_table_prefix	= ""
db_charset		= "utf8"

[Pagination]
site_per_page		= 25
site_per_pagination	= 10

[Core]
debug                   = E_ALL
super_admin             = "yes"

WYSIWYG_ALLOWED_TAGS = "<strong><em><u><a><img><p><br><ul><li><center><font>"

