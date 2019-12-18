	<!--{assign var="pluginRandParam" value=1|rand:9999}-->
	<!--{assign var="idGalleryCarousel" value="idGalleryCarousel`$plugin_gallery_id``$pluginRandParam`"}-->
	<script type="text/javascript">
	jQuery(document).ready(function() {
	    jQuery('#<!--{$idGalleryCarousel}-->').jcarousel(jcarouselOptions);
	    jQuery("#<!--{$idGalleryCarousel}--> li a.lightbox").lightBox(lightboxOptions);
	});
	</script>
	<div class="photo-gallery">
		<!--{if !empty($galleries_structure[$plugin_gallery_id])}-->
		<ul id="<!--{$idGalleryCarousel}-->" class="jcarousel-skin-tango">
		<!--{foreach from=$galleries_structure[$plugin_gallery_id] item=gp_val}-->
			<li>
				<a class="lightbox" href="/images/galleries/gallery_photos/big/<!--{$gp_val.photo_file}-->" title="<!--{$gp_val.photo_name|escape}-->"
					><img src="/images/galleries/galnews_list.lery_photos/size1/<!--{$gp_val.photo_file}-->"  alt="<!--{$gp_val.photo_name|escape}-->" /></a>
			</li>
		<!--{/foreach}-->
		</ul>
		<!--{/if}-->
	</div>