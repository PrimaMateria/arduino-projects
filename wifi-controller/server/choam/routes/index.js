var express = require("express");
var router = express.Router();
const postService = require("../services/postService");

/* GET home page. */
router.get("/", function (req, res, next) {
    const post = postService.getPost();
    res.render("index", { postPing: post.ping || "undefined" });
});

module.exports = router;
