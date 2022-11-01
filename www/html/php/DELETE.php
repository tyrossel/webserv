<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@splidejs/splide@4.1.3/dist/css/splide-core.min.css"></link>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@splidejs/splide@4.1.3/dist/css/splide.min.css"></link>
    <title>WetServ - DELETE</title>
</head>
<body>
<form action="../index.html">
    <input class="go-back" type="submit" value="<" />
</form>
<span class="header-wrap">
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1440 320"><path fill="white" fill-opacity="1" d="M0,160L8.3,165.3C16.6,171,33,181,50,197.3C66.2,213,83,235,99,213.3C115.9,192,132,128,149,138.7C165.5,149,182,235,199,272C215.2,309,232,299,248,250.7C264.8,203,281,117,298,122.7C314.5,128,331,224,348,245.3C364.1,267,381,213,397,186.7C413.8,160,430,160,447,154.7C463.4,149,480,139,497,133.3C513.1,128,530,128,546,133.3C562.8,139,579,149,596,170.7C612.4,192,629,224,646,245.3C662.1,267,679,277,695,266.7C711.7,256,728,224,745,197.3C761.4,171,778,149,794,154.7C811,160,828,192,844,208C860.7,224,877,224,894,202.7C910.3,181,927,139,943,133.3C960,128,977,160,993,149.3C1009.7,139,1026,85,1043,85.3C1059.3,85,1076,139,1092,186.7C1109,235,1126,277,1142,282.7C1158.6,288,1175,256,1192,224C1208.3,192,1225,160,1241,154.7C1257.9,149,1274,171,1291,165.3C1307.6,160,1324,128,1341,138.7C1357.2,149,1374,203,1390,224C1406.9,245,1423,235,1432,229.3L1440,224L1440,0L1431.7,0C1423.4,0,1407,0,1390,0C1373.8,0,1357,0,1341,0C1324.1,0,1308,0,1291,0C1274.5,0,1258,0,1241,0C1224.8,0,1208,0,1192,0C1175.2,0,1159,0,1142,0C1125.5,0,1109,0,1092,0C1075.9,0,1059,0,1043,0C1026.2,0,1010,0,993,0C976.6,0,960,0,943,0C926.9,0,910,0,894,0C877.2,0,861,0,844,0C827.6,0,811,0,794,0C777.9,0,761,0,745,0C728.3,0,712,0,695,0C678.6,0,662,0,646,0C629,0,612,0,596,0C579.3,0,563,0,546,0C529.7,0,513,0,497,0C480,0,463,0,447,0C430.3,0,414,0,397,0C380.7,0,364,0,348,0C331,0,314,0,298,0C281.4,0,265,0,248,0C231.7,0,215,0,199,0C182.1,0,166,0,149,0C132.4,0,116,0,99,0C82.8,0,66,0,50,0C33.1,0,17,0,8,0L0,0Z"></path></svg>
    <h1>DELETE Method</h1>
</span>
<div style="color: white; margin-top: 30px;">
<section id="image-carousel" class="splide" aria-label="Beautiful Images">
  <div class="splide__track">
  <ul class="splide__list">
<?php
    $dir = scandir("../images");
    $dir = array_diff($dir, array("..", "."));
    foreach ($dir as $item)
    {
        $item = urlencode($item); ?>
        <li class="splide__slide">
        <img src="../images/<?php echo $item; ?>">
        <button onclick="deleteClick(event)" data-filename="<?php echo $item; ?>">Delete this picture</button>
        </li>
    <?php }
?>
</ul>
  </div>
</section>
</div>
<section class="upload-wrapper">
<h2>Select a file you wish to upload to the server. The page will be refreshed automatically.<br>
You can also send a delete request on the currently selected picture by clicking on the "Delete this picture" button.</p>
    <form class="upload-form" action="upload.php" method="POST" enctype="multipart/form-data">
        <input type="file" id="upload-file" name="image"/>
        <input class="custom-file-upload" type="submit" onclick="sendClick(event)" value="UPLOAD"/>
    </form>
</section>
<section class="error-wrapper">
<button class="error-button" onclick="deleteClick(event)" data-filename="blablabla">404 Not Found</button>
</section>

<section class="svg-wrapper-b2w">
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1440 320"><path fill="black" fill-opacity="1" d="M0,192L6.2,176C12.3,160,25,128,37,138.7C49.2,149,62,203,74,213.3C86.2,224,98,192,111,202.7C123.1,213,135,267,148,261.3C160,256,172,192,185,170.7C196.9,149,209,171,222,176C233.8,181,246,171,258,181.3C270.8,192,283,224,295,213.3C307.7,203,320,149,332,144C344.6,139,357,181,369,213.3C381.5,245,394,267,406,240C418.5,213,431,139,443,122.7C455.4,107,468,149,480,160C492.3,171,505,149,517,170.7C529.2,192,542,256,554,266.7C566.2,277,578,235,591,192C603.1,149,615,107,628,101.3C640,96,652,128,665,154.7C676.9,181,689,203,702,176C713.8,149,726,75,738,64C750.8,53,763,107,775,154.7C787.7,203,800,245,812,261.3C824.6,277,837,267,849,256C861.5,245,874,235,886,218.7C898.5,203,911,181,923,149.3C935.4,117,948,75,960,64C972.3,53,985,75,997,101.3C1009.2,128,1022,160,1034,165.3C1046.2,171,1058,149,1071,144C1083.1,139,1095,149,1108,138.7C1120,128,1132,96,1145,106.7C1156.9,117,1169,171,1182,202.7C1193.8,235,1206,245,1218,213.3C1230.8,181,1243,107,1255,90.7C1267.7,75,1280,117,1292,165.3C1304.6,213,1317,267,1329,250.7C1341.5,235,1354,149,1366,106.7C1378.5,64,1391,64,1403,106.7C1415.4,149,1428,235,1434,277.3L1440,320L1440,0L1433.8,0C1427.7,0,1415,0,1403,0C1390.8,0,1378,0,1366,0C1353.8,0,1342,0,1329,0C1316.9,0,1305,0,1292,0C1280,0,1268,0,1255,0C1243.1,0,1231,0,1218,0C1206.2,0,1194,0,1182,0C1169.2,0,1157,0,1145,0C1132.3,0,1120,0,1108,0C1095.4,0,1083,0,1071,0C1058.5,0,1046,0,1034,0C1021.5,0,1009,0,997,0C984.6,0,972,0,960,0C947.7,0,935,0,923,0C910.8,0,898,0,886,0C873.8,0,862,0,849,0C836.9,0,825,0,812,0C800,0,788,0,775,0C763.1,0,751,0,738,0C726.2,0,714,0,702,0C689.2,0,677,0,665,0C652.3,0,640,0,628,0C615.4,0,603,0,591,0C578.5,0,566,0,554,0C541.5,0,529,0,517,0C504.6,0,492,0,480,0C467.7,0,455,0,443,0C430.8,0,418,0,406,0C393.8,0,382,0,369,0C356.9,0,345,0,332,0C320,0,308,0,295,0C283.1,0,271,0,258,0C246.2,0,234,0,222,0C209.2,0,197,0,185,0C172.3,0,160,0,148,0C135.4,0,123,0,111,0C98.5,0,86,0,74,0C61.5,0,49,0,37,0C24.6,0,12,0,6,0L0,0Z"></path></svg>
</section>

</body>
<script src="https://cdn.jsdelivr.net/npm/@splidejs/splide@4.1.3/dist/js/splide.min.js"></script>
<script>

  function sendClick(e) {
    e.preventDefault()
    let file = document.querySelector("#upload-file").files[0]
    let formData = new FormData()
    formData.append("image", file)
    fetch("upload.php", {method: "POST", body: formData})
    .then((data) => {
    if (data.statusText === "OK")
        window.location.href = window.location.href
    })

  }

  document.addEventListener( 'DOMContentLoaded', function () {
    new Splide( '#image-carousel' ).mount();
  } );

  function deleteClick(e) {
    let url = `http://${window.location.host}/images/${e.target.getAttribute("data-filename")}`
    console.log(window.location)
    console.log(url)

        let response = fetch(url, {
            credentials: 'include',
            headers: new Headers({
                'Content-Type': 'Application/json',
                'Accept': 'Application/json',
            }),
            method: 'DELETE',
        })
        .then((data) => {
            if (data.statusText === "OK")
                window.location.href = window.location.href
        })
  }
</script>
</html>

<style lang="css">
.error-wrapper {
    display: flex;
    justify-content: center;
}
.error-button {
    background: none;
    border: 2px white solid;
    color: white;
    cursor: pointer;
}
.upload-wrapper {
    display: flex;
    flex-direction: row;
    justify-content: center;
    color: white;
    text-align: center;
}

.upload-form input {
    background: none;
    border: 2px solid white;
    color: white;
    cursor: pointer;
    margin: 20px;
}

.upload-form input[type="file"] {
    background: none;
    border: none;
    color: white;
    cursor: pointer;
}

.splide__slide {
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    margin-bottom: 40px;
    gap: 20px;
}
.splide__slide img {
    max-width: 85vw;
}
.splide__slide button {
    background: none;
    border: 2px solid white;
    color: white;
    cursor: pointer;
}
    .go-back {
        color: black;
        font: inherit;
        background: white;
        font-weight: bolder;
        font-size: 30px;
        position: absolute;
        border: none;
        padding: 12px;
        cursor: pointer;
        outline: inherit;
    }
    @keyframes left-right {
        0% {left: 0px;}
        50% {left: -5px;}
        100% {left: 0px;}

    }
    .go-back:hover {
        color: #000A;
        animation-name: left-right;
        animation-duration: 1s;
        animation-iteration-count: infinite;
    }
    body {
        background: black;
        margin: 0;
    }
    .header-wrap {
        display: flex;
        justify-content: center;
        margin-bottom: 200px;
    }
    .header-wrap svg {
        position: absolute;
        z-index: -10;
    }
    .svg-wrapper-b2w {
        background: white;
    }
</style>