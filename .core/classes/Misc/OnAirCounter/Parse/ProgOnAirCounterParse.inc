<?php
namespace Ekv\Misc\OnAirCounter\Parse;

/**
 * Calculates how many times a specific prog has been used on air at each channel
 */
class ProgOnAirCounterParse
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
            $parserObj = new ProgOnAirCounterParseChannel($channelVal["id_channel"]);
            $parserObj->mainParseChannel();
        }
    }
}
