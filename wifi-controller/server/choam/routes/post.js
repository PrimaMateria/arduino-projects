var express = require("express");
const { post } = require(".");
var router = express.Router();
const postService = require("../services/postService");

router.post("/ping", function (req, res, next) {
    postService.setPostPing(new Date());
    res.send("ok");
});

router.put("/status/:status", function (req, res, next) {
    const status = req.params["status"] === "true";
    postService.setPostStatus(status);
    res.send("ok");
});

module.exports = router;
