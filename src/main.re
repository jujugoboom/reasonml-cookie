type element;

type context;

type image;

[@bs.scope "document"] [@bs.return nullable] [@bs.val]
external getElementById : string => option(element) = "";
[@bs.set] external setWidth : (element, int) => unit = "width";
[@bs.set] external setHeight : (element, int) => unit = "height";

[@bs.get] external innerWidth : (element) => int = "";
[@bs.set] external onresize : (element, (unit) => unit) => unit = "";

[@bs.scope "Math"] [@bs.val]
external random : unit => float = "";

[@bs.send] external getContext : (element, string) => context = "";
[@bs.send] external _rect : (context, int, int, int, int) => unit = "rect";
[@bs.send] external fill : (context) => unit = "";
[@bs.send] external drawCanvas : (context, element, int, int, int, int, float, int, float, int) => unit = "drawImage";
[@bs.send] external drawImage : (context, image, int, int, int, int, float, int, float, int) => unit = "";

[@bs.new] external createImage : unit => image = "Image";
[@bs.get] external width : (image) => int = "";
[@bs.get] external height : (image) => int = "";
[@bs.set] external setSrc : (image, string) => unit = "src";
[@bs.set] external onload : (image, (unit) => unit) => unit = "";

[@bs.val] external setInterval : (unit => unit, int) => float = "";
[@bs.val] external clearInterval : float => unit = "";
[@bs.val] external setTimeout : (unit => unit, int) => float = "";
[@bs.val] external clearTimeout : float => unit = "";

let canvas = switch (getElementById("canvas")) {
    | None => raise(Not_found)
    | Some(e) => e
};
let context = getContext(canvas, "2d");
let img = createImage();
let w = ref(0);
let h = ref(0);
let glitchInterval = ref(0.0);
let offset = ref(0.0);

let window: element = [%raw "window"];

let clear = () => {
    _rect(context, 0, 0, w^, h^);
    fill(context);
};

let rec for_ = (d, c, i, f) => {
    if (c(d)){
        f();
        for_(i(d), c, i, f);
    }
};

let randInt = (a, b) => {
    truncate(random() *. (float(b) -. float(a)) +. float(a));
};

let glitchImg = () => {
    for_(0, (i) => {i < randInt(1, 13)}, (i) => {i + 1}, () => {
        let x = random() *. float(w^);
        let y = random() *. float(h^);
        let spliceWidth = w^ - h^;
        let spliceHeight = randInt(5, h^ / 3);
        drawCanvas(context, canvas, 0, truncate(y), spliceWidth, spliceHeight, x, truncate(y), float(spliceWidth), spliceHeight);
        drawCanvas(context, canvas, spliceWidth, truncate(y), truncate(x), spliceHeight, 0.0, truncate(y), x, spliceHeight);
    });
};

let init = () => {
    clearInterval(glitchInterval^);
    w := innerWidth(window);
    setWidth(canvas, w^);
    offset := float(w^) *. 0.1;
    h :=  (175 * ((w^ - (truncate(offset^) * 2)) / width(img)));
    setHeight(canvas, h^);
    glitchInterval := setInterval(() => {
        clear();
        drawImage(context, img, 0, 0, width(img), height(img), offset^, 0, float(w^) -. (offset^ *. 2.0), h^);
        let _ = setTimeout(glitchImg, randInt(250, 1000));
    }, 500);
};

onload(img, () => {
    init();
    onresize(window, init);
});
setSrc(img, "http://www.freepngimg.com/download/cookie/2-2-cookie-transparent.png")
