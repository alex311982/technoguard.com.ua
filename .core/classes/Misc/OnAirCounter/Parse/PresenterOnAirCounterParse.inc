<?php
namespace Ekv\Misc\OnAirCounter\Parse;

/**
 * Calculates how many times specific presenter has been used on air at each channel
 */
class PresenterOnAirCounterParse
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    public function mainParse()
    {
        $chanObj = new \EkvChannel();
        $channels = $chanObj->getChannels();

        foreach ($channels as $channelVal) {
            $parserObj = new PresenterOnAirCounterParseChannel($channelVal["id_channel"]);
            $parserObj->mainParseChannel();
        }
    }
}
