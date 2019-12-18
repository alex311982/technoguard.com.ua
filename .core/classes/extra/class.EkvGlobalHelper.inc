<?php

class EkvGlobalHelper
{
    const
        TABLE_CHANNEL = "ekv_channel",
        TABLE_CHANNEL_ONLINE_URLS = "ekv_channel_online_urls",

        TABLE_SCHEDULE_TPL_ROOT = "ekv_schedule_tpl",
        TABLE_SCHEDULE_TPL_ITEM = "ekv_schedule_tpl_item",
        TABLE_SCHEDULE_PERIOD = "ekv_schedule_period",
        TABLE_SCHEDULE_PERIOD_ITEM = "ekv_schedule_period_item",
        TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE = "ekv_reparse_queue",

        TABLE_PROG_PLAYBACK_TYPE = "ekv_prog_playback_type",
        TABLE_LANG = "ekv_lang",

        TABLE_MODER = "ekv_moderator",
        TABLE_MODER_CHANNEL_ACCESS = "ekv_moderator_channel_access",

        TABLE_PROG_PRESENTER = "ekv_prog_presenter",
        TABLE_PROG = "ekv_prog",
        TABLE_PROG_PRESENTER2TPL_ITEM = "ekv_prog_presenter2tpl_item",
        TABLE_PROG_PRESENTER2PERIOD_ITEM = "ekv_prog_presenter2period_item",

        TABLE_BASE_USER = "base_user", // legacy table for backend super admins
        TABLE_USER_BACKEND = "ekv_user_backend",

        //--- news ---//
        TABLE_NEWS_BASE = "ekv_news",
        TABLE_NEWS_TRANSL = "ekv_news_translation",
        TABLE_NEWS_EDITOR_SAVED_COUNT = "ekv_news_save_editor",

        //--- static page ---//
        TABLE_STAT_PAGE_BASE = "ekv_stat_page",
        TABLE_STAT_PAGE_TRANSL = "ekv_stat_page_translation",

        //--- translatable content ---//
        TABLE_TCONTENT_TAGS = "ekv_tc_tags",
        TABLE_TCONTENT_TAGS_CONN = "ekv_tc_tags_conn",

        TABLE_MENU_FRONT = "ekv_menu_front",
        TABLE_SLIDE = "ekv_slide",
        TABLE_BANNER = "ekv_banner",
        TABLE_SKIN_SCHEDULE = "ekv_skin_schedule",

        TABLE_PROG_ONAIR_USAGE = "ekv_prog_onair_usage",
        TABLE_PRESENTER_ONAIR_USAGE = "ekv_presenter_onair_usage",

        //--- deprecated below ---//
        TABLE_STAFF = "ekv_staff",
        TABLE_STAFF_DEPARTMENT = "ekv_staff_department",
        TABLE_STAFF_TO_DEPARTMENT = "ekv_staff2department"
    ;

    /**
     * grid playback in progress, оформление
     * @see TABLE_PROG_PLAYBACK_TYPE, field id=5
     */
    const PLAYBACK_TYPE_IN_EDITION = 5;

    const PLAYBACK_TYPE_LIVE = 2;

}
