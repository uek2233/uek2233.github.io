var posts=["posts/4a17b156.html","posts/9aa43288.html","posts/79666db.html","posts/51f321c6.html","posts/673b0eac.html","posts/4232bc1d.html","posts/a817377e.html","posts/77731701.html","posts/93a4bc9e.html","posts/aa11dae7.html","posts/aa11dae6.html","posts/9c300b3c.html","posts/c8d982b8.html","posts/81737f40.html","posts/9cfaa3ca.html"];function toRandomPost(){pjax.loadUrl('/'+posts[Math.floor(Math.random() * posts.length)]);};