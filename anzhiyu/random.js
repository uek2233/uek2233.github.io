var posts=["posts/4a17b156.html","posts/4232bc1d.html","posts/673b0eac.html","posts/77731701.html","posts/aa11dae7.html","posts/c8d982b8.html","posts/aa11dae6.html","posts/81737f40.html"];function toRandomPost(){pjax.loadUrl('/'+posts[Math.floor(Math.random() * posts.length)]);};