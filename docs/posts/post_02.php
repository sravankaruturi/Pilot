<?php
include_once "post_header.php";
?>

    <!-- Page Header -->
    <header class="masthead" style="background-image: url('img/post-bg.jpg')">
      <div class="overlay"></div>
      <div class="container">
        <div class="row">
          <div class="col-lg-8 col-md-10 mx-auto">
            <div class="post-heading">
              <h1>Raypicking</h1>
              <h2 class="subheading">..and Unit Tests</h2>
              <span class="meta">Posted by
                <a href="#">Sravan</a>
                on September 15, 2018</span>
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
            <p>This is interesting. I have never needed to implement Ray Picking before since, I never made a Game in OpenGL or DirectX where you would point and click. I wasn't familiar with the concepts and loic behind it.</p>

            <p>I dove in, read some online material on it, of which <a href="http://antongerdelan.net/opengl/raycasting.html" target="_blank">this</a> stood out the most to me. This is nicely summarized in <a href="https://www.youtube.com/watch?v=DLKN0jExRIM" target="_blank">a video</a> althought the video doesn't use C++</p>

            <p>I had a lot of trouble with Ray Picking. I was quite confused as to why it did not work even though I did everything right. And after a couple of days of being stuck, I decided to write Unit Tests for this Engine to help me Debug where the issue was.</p>

            <h2 class="section-heading">Unit Tests</h2>

            <p>Once I decided to include Unit Tests in my Project, I wanted to make sure that I would only write the tests as needed due to time constraints. So, this project would not be guided by the Test Driven Development Principles. But, the Unit Tests included would help me debug the Program. This might come back to impact me in the future, but for now, this seems to work just fine.</p>
              
            <p>Another issue I faced back then is to get the exact values of each Ray, Model, View Matrices to help write Unit Tests. I got it after some manual calculation and whiteboarding. You can see most of it the program, under the CameraTests.cpp file. </p>

            <!--<blockquote class="blockquote">The dreams of yesterday are the hopes of today and the reality of tomorrow. Science has not yet mastered prophecy. We predict too much for the next year and yet far too little for the next ten.</blockquote>-->

            <p>I also had the issue of determining the boundaries of a given mesh, and to use those boundaries for Ray Picking. I just a cube for now, But Bounding Boxes and determining their collisions with a Ray were introduced here.</p>

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