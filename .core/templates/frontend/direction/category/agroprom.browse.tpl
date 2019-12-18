<div class="container">
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

<main class="heading">
    <div class="container">
        <div class="heading__title">
            <h1><!--{$header.direction_title}--> <span>/</span></h1>
            <h2><!--{$header.title}--></h2>
        </div>
    </div>
    <div class="heading__image">
        <img src="<!--{$header.imagePath}-->" alt="Agroprom Image">
    </div>
</main>

<section class="description">
    <div class="container smaller">
        <div class="description__inner">
            <div class="description__block">
                <p>Аграрная промышленность —
                    основа украинской экономики.</p>
                <p>И для развития агропрома (АПК) необходимо внедрение инноваций. На производстве продукции,
                    для ее учета, хранения
                    и транспортировки. </p>
                <p class="special"><span>/</span>«Техногард» предлагает широкий набор решений, на базе RFID для решения задач
                    на агропромышленных предприятиях малого, среднего
                    и крупного масштаба.</p>
            </div>
            <div class="description__block">
                <p>Мы проектируем, устанавливаем
                    и обслуживаем АПК-объекты — склады, цеха и административные здания.
                </p>
                <p class="special"><span>/</span>«Техногард» работает с лучшими производителями оборудования.
                    Мы лично проверяем качество
                    и соответствие всех комплектующих.
                </p>
                <p>Наши эксперты при проектировании оптимизируют затраты заказчика. Многолетний опыт и наработанная экспертиза
                    дает стабильный, эффективный результат для бизнеса.</p>
            </div>
        </div>
    </div>
</section>

<!--{if !empty($decisions)}-->
<section class="benefits">
    <div class="container">
        <div class="benefits__title">
            <h3>Наши решения для хранения
                и перевозки продукции <span class="line">/</span> <span class="grey">в агропроме</span></h3>
        </div>
        <div class="benefits__main">
            <!--{foreach from=$decisions item=decision}-->
            <div class="benefits__single special">
                <a href="#decision-<!--{$decision.id}-->" data-scroll="decision-<!--{$decision.id}-->">
                    <!--{$decision.title}-->
                </a>
            </div>
            <!--{/foreach}-->
        </div>

        <!--{foreach from=$decisions item=decision}-->
        <div id="decision-<!--{$decision.id}-->" class="tab__content no-mobile">
            <div class="tab__main left-position">
                <h2><!--{$decision.title}--></h2>
                <p><!--{$decision.text}--></p>
                <a href="/decision.html?id=<!--{$decision.id}-->">
                    <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                    Выбрать решение
                </a>
            </div>
            <img class="tab__image" src="<!--{$decision.imagePath}-->" alt="tab 1">
        </div>
        <!--{/foreach}-->

        <section class="tabs-mobile">
            <!--{foreach from=$decisions item=decision}-->
            <div data-scroll="decision-<!--{$decision.id}-->" class="tabs-mobile-content tabs-mobile-subpage">
                <h2><!--{$decision.title}--></h2>
                <img class="tab__image" src="<!--{$decision.imagePath}-->" alt="tab 1">
                <p><!--{$decision.text}--></p>
                <a href="/decision.html?id=<!--{$decision.id}-->">
                    <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                    Выбрать решение
                </a>
            </div>
            <!--{/foreach}-->
        </section>
    </div>
</section>
<!--{/if}-->

<!--{if !empty($news)}-->
    <section class="cases">
        <div class="container">
            <h2>Наш опыт внедрения решений для ритейла</h2>
            <div class="cases__items">
                <!--{foreach from=$news item=news_item}-->
                <a href="/news.html?id=<!--{$news_item.id}-->" class="cases__item">
                    <img style="width:255px; height:254px" src="<!--{$news_item.imagePath}-->" alt="adv 1">
                    <p><!--{$news_item.news_title}--></p>
                </a>
                <!--{/foreach}-->
            </div>
        </div>
    </section>
<!--{/if}-->

<section class="contact">
    <div class="container">
        <div class="contact-form">
            <img src="/-images/new/assets/images/form-photo.png" alt="contact form photo">
            <div class="inner-form">
                <h3>Спросите нас</h3>
                <form action="./index.js">
                    <input required placeholder="Ваше имя" aria-placeholder="Ваше имя" type="text" name="name" id="name">
                    <input required placeholder="Email" aria-placeholder="Email" type="email" name="email" id="email">
                    <textarea placeholder="Опишите ваш запрос в двух словах" aria-placeholder="Опишите ваш запрос в двух словах"
                              name="about" id="about" rows="10"></textarea>
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
                    <li><a href="#">Facebook</a></li>
                    <li><a href="/contact">Контакты</a></li>
                </ul>
            </nav>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
            <img class="footer-logo" src="/-images/new/assets/images/bw_logo.svg" alt="Technoguard logo">
        </div>
    </div>
</footer>