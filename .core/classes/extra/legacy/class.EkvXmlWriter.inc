<?php
/**
 * User: sunsey
 * Date: 19 лип 2011
 */

class EkvXmlWriter
{
    public $xw = null;
    public function __construct()
    {
        $this->xw = new XMLWriter();
        $this->xw->openURI('php://output');
        $this->xw->setIndent(true);
    }

    function startXML($doc_encoding = "utf-8")
    {
        //$this->xw->startDocument('1.0', $doc_encoding);
        /**
         * very strange hack, but it works
         * http://www.php.net/manual/en/function.xmlwriter-start-document.php#89957
         */
        //$this->xw->startDocument( '1.0" encoding="windows-1251' ); // use this hardcode for windows-1251 !!!
        $this->xw->startDocument('1.0', "utf-8");

    }

    function endXML()
    {
        $this->xw->endDocument();
        $this->xw->flush();
    }

    function element($tag, $attrs = null, $content = null)
    {
        $this->elementStart($tag, $attrs);
        if (!is_null($content)) {
            $this->xw->text($content);
            //$this->xw->writeRaw($content);
        }
        $this->elementEnd($tag);
    }
    function elementRaw($tag, $attrs = null, $content = null)
    {
        $this->elementStart($tag, $attrs);
        if (!is_null($content)) {
            //$this->xw->text($content);
            $this->xw->writeRaw($content);
        }
        $this->elementEnd($tag);
    }

    function elementCDATA($tag, $attrs = null, $content = null)
    {
        $this->elementStart($tag, $attrs);
        if (!is_null($content)) {
            //$this->xw->text($content);
            $this->xw->writeCdata($content);
        }
        $this->elementEnd($tag);
    }

    function elementStart($tag, $attrs = null)
    {
        $this->xw->startElement($tag);
        if (is_array($attrs)) {
            foreach ($attrs as $name => $value) {
                $this->xw->writeAttribute($name, $value);
            }
        } elseif (is_string($attrs)) {
            $this->xw->writeAttribute('class', $attrs);
        }
    }

    function elementEnd($tag)
    {
        static $empty_tag = array('base', 'meta', 'link', 'hr', 'br', 'param', 'img', 'area', 'input', 'col');
        if (in_array($tag, $empty_tag)) {
            $this->xw->endElement();
        } else {
            $this->xw->fullEndElement();
        }
        if(LIVE_VERSION == "off"){
            $this->eol();
        }
    }

    function text($txt)
    {
        $this->xw->text($txt);
    }

    function raw($xml)
    {
        $this->xw->writeRaw($xml);
    }

    function comment($txt)
    {
        $this->xw->writeComment($txt);
    }

    function eol()
    {
        $this->xw->writeRaw(PHP_EOL);
        //echo PHP_EOL;
    }

}
