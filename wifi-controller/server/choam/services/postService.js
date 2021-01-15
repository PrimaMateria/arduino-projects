const low = require("lowdb");
const FileSync = require("lowdb/adapters/FileSync");

const adapter = new FileSync("db.json");
const db = low(adapter);

db.defaults({ post: { ping: undefined, status: false } });

const setPostPing = (ts) => {
    db.set("post.ping", ts).write();
};

const getPost = () => {
    return db.get("post").value();
};

const setPostStatus = (status) => {
    setPostPing(new Date());
    db.set("post.status", status).write();
};

module.exports = {
    setPostPing: setPostPing,
    getPost: getPost,
    setPostStatus: setPostStatus,
};
