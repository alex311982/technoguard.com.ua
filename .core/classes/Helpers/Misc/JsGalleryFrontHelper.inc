<?php
namespace Ekv\Helpers\Misc;

use Entity;
use GalleryMultiUpload;

class JsGalleryFrontHelper
{
    private $DB;
    /**
     * @var Entity
     */
    private $en;

    private $jsOptions = array(
        'visible' => 3,
        'scroll' => 2
    );

    function __construct(Entity $en)
    {
        $this->DB = \DB::getInstance();
        $this->en = $en;
    }

    function setOptionVisible($val)
    {
        $this->jsOptions["visible"] = $val;
    }

    function mainParse($body)
    {
        GalleryMultiUpload::initDb($this->DB);

        GalleryMultiUpload::assignGalleryOptions($this->en, $this->jsOptions["visible"], $this->jsOptions["scroll"]);

        $body = GalleryMultiUpload::replaceGalleryTagsExt($body, $this->en);

        return $body;
    }

    public function mainParseRef(&$bodyRef)
    {
        $bodyRef = $this->mainParse($bodyRef);
    }
}
 