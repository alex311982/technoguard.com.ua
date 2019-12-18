<?php
namespace Ekv\TContent\Slide;

class SlideSpecialProjectsFormatter extends SlideFormatter
{
    function __construct()
    {
        parent::__construct(SlideRoot::SLIDE_TYPE_SPECIAL_PROJECTS);
        $this->setImgSize('size1');
    }

}
