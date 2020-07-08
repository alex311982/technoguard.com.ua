<div class="container smaller">
    <header class="header extra-padding">
        <a href=".">
            <img src="/-images/new/assets/images/technoguard_logo.svg" alt="Technoguard Logo">
        </a>

        <button aria-label="Toggle menu" class="toggle-menu">
            <span></span>
            <span></span>
            <span></span>
        </button>

        <nav role="navigation" class="menu-mobile">
            <img src="/-images/new/assets/images/technoguard_logo.svg" alt="Technoguard Logo">
            <ul>
                <li><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
                <li class="active"><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect cselect--phone">
                <div data-select="<!--{$selectedLangCode}-->" class="cselect__selected"><!--{$selectedLang}--></div>
                <ul class="cselect__list">
                    <li data-select="<!--{$itemLangCode}-->" class="cselect__item"><!--{$itemLang}--></li>
                </ul>
              <svg class="cselect__arrow" xmlns="http://www.w3.org/2000/svg" width="8.828" height="5.828" viewBox="0 0 8.828 5.828">
                <g id="Group_30" data-name="Group 30" transform="translate(1.414 1.414)" opacity="0.5">
                  <line id="Line_12" data-name="Line 12" x2="3" y2="3" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                  <line id="Line_13" data-name="Line 13" x1="3" y2="3" transform="translate(3)" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                </g>
              </svg>
            </div>
        </nav>

        <nav role="navigation" class="menu">
            <ul>
                <li><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
                <li class="active"><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect">
                <div data-select="<!--{$selectedLangCode}-->" class="cselect__selected"><!--{$selectedLang}--></div>
                <ul class="cselect__list">
                    <li data-select="<!--{$itemLangCode}-->" class="cselect__item"><!--{$itemLang}--></li>
                </ul>
              <svg class="cselect__arrow" xmlns="http://www.w3.org/2000/svg" width="8.828" height="5.828" viewBox="0 0 8.828 5.828">
                <g id="Group_30" data-name="Group 30" transform="translate(1.414 1.414)" opacity="0.5">
                  <line id="Line_12" data-name="Line 12" x2="3" y2="3" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                  <line id="Line_13" data-name="Line 13" x1="3" y2="3" transform="translate(3)" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                </g>
              </svg>
            </div>
        </nav>
    </header>
</div>

<section class="contact-section">
    <div class="container smaller no-padding">
        <div class="contact-section__title">
            <h1><!--{$translations.global_footer_contacts}--> </h1>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
        </div>
        <div class="contact-section__image">
            <img src="/-images/new/assets/images/contact-image.png" alt="Contact image">
        </div>
    </div>
</section>

<section class="find-us">
    <div class="container smaller">
        <div class="find-us__block">
            <div class="find-us__info">
                <span><!--{$translations.contact_company}--></span>
                <p><!--{$translations.contact_company_name}--></p>
            </div>
            <div class="find-us__block--middle">
                <div class="find-us__info">
                    <span><!--{$translations.contact_company_phone}--></span>
                    <a href="tel:+380444566606">+38 044 456 66 06</a>
                </div>
                <div class="find-us__info extra-padding-left">
                    <span>E-mail:</span>
                    <a href="mailto:security@technoguard.com.ua">Security@technoguard.com.ua</a>
                </div>
            </div>
            <div class="find-us__info">
                <span><!--{$translations.contact_company_address}--></span>
                <p><!--{$translations.contact_company_full_address}--></p>
            </div>
        </div>
    </div>
</section>

<div id="map"></div>

<section class="contact">
    <div class="container">
        <div class="contact-form">
            <img src="/-images/new/assets/images/form-photo.png" alt="contact form photo">
            <div class="inner-form">
                <h3><!--{$translations.global_ask_us}--></h3>
                <form action="./index.js">
                    <input required placeholder="<!--{$translations.global_ask_us_your_name}-->" aria-placeholder="<!--{$translations.global_ask_us_your_name}-->" type="text" name="name" id="name">
                    <input required placeholder="Email" aria-placeholder="Email" type="email" name="email" id="email">
                    <textarea placeholder="<!--{$translations.global_ask_us_question}-->"
                              aria-placeholder="<!--{$translations.global_ask_in_2_words}-->" name="about" id="about" rows="10"></textarea>
                    <button class="form-button" type="submit"><!--{$translations.global_ask_us_button_send}--></button>
                </form>
                <div class="success-block">
                    <img src="/-images/new/assets/icons/ic_mark.svg" alt="Mark icon">
                    <p><!--{$translations.global_ask_us_thanks}--></p>
                </div>
            </div>
        </div>
    </div>
</section>

<footer class="footer">
    <div class="container">
        <div class="footer__inner">
            <nav class="footer__menu">
                <ul>
                    <li><a href="https://www.facebook.com/TechnoguardUA/">Facebook</a></li>
                    <li><a href="/<!--{$lang}-->/contact"><!--{$translations.global_footer_contacts}--></a></li>
                </ul>
            </nav>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
            <img class="footer-logo" src="/-images/new/assets/images/bw_logo.svg" alt="Technoguard logo">
        </div>
    </div>
</footer>
<script>
    function initMap() {
        const technoguard = { lat: 50.504005, lng: 30.46785 };
        const map = new google.maps.Map(
            document.getElementById('map'), { zoom: 17, center: technoguard });
        const marker = new google.maps.Marker({
            position: technoguard,
            map,
            icon: 'https://i.ibb.co/S5j6B4v/tech-logo.png'
        });
    }
</script>
<script async defer
        src="https://maps.googleapis.com/maps/api/js?key=AIzaSyBK70ZvsjcPtq0-lb4sRwASHNTB5Gbh12A&callback=initMap">
</script>
