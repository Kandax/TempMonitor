const express = require("express");

const app = express();

app.use(express.json());


app.post("/temperature", (req, res) => {

    console.log(req.body);

    res.send("OK");
});


app.listen(3000, () => {
    console.log("Server działa");
});