var express = require("express");
const { post } = require(".");
var router = express.Router();
const postService = require("../services/postService");

router.post("/", function (req, res, next) {
    postService.setPostPing(new Date());
    res.send("ok");
});

module.exports = router;
