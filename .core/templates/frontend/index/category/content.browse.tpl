<div class="container smaller">
    <header class="header">
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
                <li><a href="/about">О компании</a></li>
                <li><a href="/contact">Контакты</a></li>
            </ul>
        </nav>

        <nav role="navigation" class="menu">
            <ul>
                <li><a href="/about">О компании</a></li>
                <li><a href="/contact">Контакты</a></li>
            </ul>
        </nav>
    </header>
</div>

<section class="main">
    <!--{if !empty($banners)}-->
        <div class="slider">
            <!--{foreach from=$banners item=banner}-->
                <div class="slide">
                    <div class="slide__image">
                        <img src="<!--{$banner.imagePath}-->" alt="Slider image">
                        <a href="<!--{$banner.url}-->">
                            Узнать
                        </a>
                    </div>
                    <div class="slide__right">
                        <div>
                            <span><!--{$banner.subtitle}--></span>
                            <h1><!--{$banner.title}--></h1>
                        </div>
                    </div>
                </div>
            <!--{/foreach}-->
        </div>
    <!--{/if}-->
    <div class="current-slide-number"></div>
</section>

<div class="container">
    <section class="tabs">

        <ul class="tabs__caption">
            <li class="active"><a href="#retail" data-scroll="retail">Ритейл</a></li>
            <li><a href="#agroprom" data-scroll="agroprom">Агропром</a></li>
            <li><a href="#rfid" data-scroll="rfid">RFID</a></li>
        </ul>

        <div class="tabs__content active">

            <div class="tab__content">
                <div id="retail" class="tab__main left-position">
                    <h2>Ритейл</h2>
                    <p>Защищаем предприятия розничной
                        и оптовой торговли от краж и порчи
                        с комфортом для персонала
                        и покупателя</p>
                    <a href="/direction.html?type=retail">
                        <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                        Выбрать решение для ритейла
                    </a>
                </div>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
            </div>
            <div class="tab__content">
                <img class="tab__image" src="/-images/new/assets/images/tab-2.png" alt="tab 1">
                <div id="agroprom" class="tab__main right-position">
                    <h2>Агропром</h2>
                    <p>Проектируем, устанавливаем и обслуживаем решения, на базе технологии RFID для учета, хранения
                        и транспортировки сельхозпродукции</p>
                    <a href="/direction.html?type=agroprom">
                        Выбрать решение для агропрома
                        <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                    </a>
                </div>
            </div>
            <div class="tab__content">
                <div id="rfid" class="tab__main left-position">
                    <h2>RFID</h2>
                    <p>Защищаем предприятия розничной
                        и оптовой торговли от краж и порчи
                        с комфортом для персонала
                        и покупателя</p>
                    <a href="/direction.html?type=RFID">
                        <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                        Выбрать решение для RFID
                    </a>
                </div>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
            </div>
        </div>
    </section>
    <section class="tabs-mobile">
        <div data-scroll="retail">
            <a href="/direction.html?type=retail" class="tabs-mobile-content">
                <h2>Ритейл</h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
                <p>Проектируем, устанавливаем и обслуживаем решения, на базе технологии RFID для учета, хранения
                    и транспортировки сельхозпродукции</p>
            </a>
        </div>
        <div data-scroll="agroprom">
            <a href="/direction.html?type=agroprom" class="tabs-mobile-content">
                <h2>Агропром</h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-2.png" alt="tab 1">
                <p>Проектируем, устанавливаем и обслуживаем решения, на базе технологии RFID для учета, хранения
                    и транспортировки сельхозпродукции</p>
            </a>
        </div>
        <div data-scroll="rfid">
            <a href="/direction.html?type=RFID" class="tabs-mobile-content">
                <h2>RFID</h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
                <p>Проектируем, устанавливаем и обслуживаем решения, на базе технологии RFID для учета, хранения
                    и транспортировки сельхозпродукции</p>
            </a>
        </div>
    </section>
</div>

<section class="contact">
    <div class="container">
        <div class="contact-form">
            <img src="/-images/new/assets/images/form-photo.png" alt="contact form photo">
            <div class="inner-form">
                <h3>Спросите нас</h3>
                <form action="./index.js">
                    <input required placeholder="Ваше имя" aria-placeholder="Ваше имя" type="text" name="name" id="name">
                    <input required placeholder="Email" aria-placeholder="Email" type="email" name="email" id="email">
                    <textarea placeholder="Опишите ваш запрос в двух словах"
                              aria-placeholder="Опишите ваш запрос в двух словах" name="about" id="about" rows="10"></textarea>
                    <button class="form-button" type="submit">Отправить запрос</button>
                </form>
                <div class="success-block">
                    <img src="/-images/new/assets/icons/ic_mark.svg" alt="Mark icon">
                    <p>Спасибо за ваше обращение.
                        Мы постараемся ответить быстро!</p>
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
                    <li><a href="/contact">Контакты</a></li>
                </ul>
            </nav>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
            <img class="footer-logo" src="/-images/new/assets/images/bw_logo.svg" alt="Technoguard logo">
        </div>
    </div>
</footer>