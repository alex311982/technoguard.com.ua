<?php
namespace Ekv\TContent\Slide;

class SlideBottomFormatter extends SlideFormatter
{
    function __construct()
    {
        parent::__construct(SlideRoot::SLIDE_TYPE_BOTTOM);
        $this->setImgSize('size1');
    }

}
