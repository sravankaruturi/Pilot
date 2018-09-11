<?php
include_once "post_header.php";
?>

  <body>

    <!-- Navigation -->
    <nav class="navbar navbar-expand-lg navbar-light fixed-top" id="mainNav">
      <div class="container">
        <a class="navbar-brand" href="../index.html">Pilot Engine</a>
        <button class="navbar-toggler navbar-toggler-right" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
          Menu
          <i class="fas fa-bars"></i>
        </button>
        <div class="collapse navbar-collapse" id="navbarResponsive">
          <ul class="navbar-nav ml-auto">
            <li class="nav-item">
              <a class="nav-link" href="../index.html">Home</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="../about.html">About</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="../post.html">Sample Post</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="../contact.html">Contact</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>

    <!-- Page Header -->
    <header class="masthead" style="background-image: url('img/post-bg.jpg')">
      <div class="overlay"></div>
      <div class="container">
        <div class="row">
          <div class="col-lg-8 col-md-10 mx-auto">
            <div class="post-heading">
              <h1>Initial Commit</h1>
              <h2 class="subheading">How I started the project</h2>
              <span class="meta">Posted by
                <a href="#">Sravan</a>
                on September 11, 2018</span>
            </div>
          </div>
        </div>
      </div>
    </header>

    <!-- Post Content -->
    <article>
      <div class="container">
        <div class="row">
          <div class="col-lg-8 col-md-10 mx-auto">
            <p>I have been trying to stick to an Engine for a while now. And everytime, I fallback on creating a new one because I didn't like the Old one, after a certain point. This is the same. It started out while I was reading a book, and following his examples. Halfway through it, I did not like where it was going. So, I scrapped it, and started new. This time, with the knowledge of what I needed to implement first.</p>

            <p>This Engine is supposed to be an Engine for an RTS Game. Now, how it would turn out, is to be seen. But so, far, I'm satisfied with the results</p>

            <p>I've decided to take elaborate notes and document the entire process a week after I started this Engine. So, I do not have the very start of the process.</p>

            <h2 class="section-heading">Terrain Loader</h2>

            <p>I realized I wanted to start with loading a terrain. Load a heightmap file and create a terrain based on that. This way, I can create my own terrain in tools like Photoshop or Gimp without leaving the map upto chance, which is a huge deal when it comes to game balance. Note that it does however means that we need to create every map in the game ourselves.</p>

            <!--<blockquote class="blockquote">The dreams of yesterday are the hopes of today and the reality of tomorrow. Science has not yet mastered prophecy. We predict too much for the next year and yet far too little for the next ten.</blockquote>-->

            <p>Actual development of the Terrain was easy, however the whiteboarding process took a while. Right now, every scene has a Terrain. It could be a nullptr, but it exists. This is to make sure that all Scenes conform to a specific set of Rules.</p>

            <!--<h2 class="section-heading">Reaching for the Stars</h2>-->

            <!--<a href="#">-->
              <!--<img class="img-fluid" src="../img/post-sample-image.jpg" alt="">-->
            <!--</a>-->
            <!--<span class="caption text-muted">To go places and do things that have never been done before – that’s what living is all about.</span>-->

            <!--<p>Space, the final frontier. These are the voyages of the Starship Enterprise. Its five-year mission: to explore strange new worlds, to seek out new life and new civilizations, to boldly go where no man has gone before.</p>-->

            <!--<p>As I stand out here in the wonders of the unknown at Hadley, I sort of realize there’s a fundamental truth to our nature, Man must explore, and this is exploration at its greatest.</p>-->

            <!--<p>Placeholder text by-->
              <!--<a href="http://spaceipsum.com/">Space Ipsum</a>. Photographs by-->
              <!--<a href="https://www.flickr.com/photos/nasacommons/">NASA on The Commons</a>.</p>-->
          <!--</div>-->
        </div>
      </div>
    </article>

    <hr>

<?php
include_once "post_footer.php";
?>