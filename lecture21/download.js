const fs = require('fs');
const crossFetch = require('cross-fetch');
const realIsomorphicFetch = require('real-isomorphic-fetch');

const url = [
    'https://github.com/LearningOS/os-lectures/tree/master/lecture21/slide-21-xx.tex',
    'https://cloud.tsinghua.edu.cn/d/339de5f93d8c420ebbb7/files/?p=%2Flecture21%2Fslide-21-xx.mp4&dl=1',
    'http://os.cs.tsinghua.edu.cn/oscourse/OS2020spring/lecture21?action=AttachFile&do=get&target=slide-21-xx.pdf',
];

const fileName = [
    'slide-21-xx.tex',
    'slide-21-xx.mp4',
    'slide-21-xx.pdf',
];

(async () => {
    let tasks = [];
    for (let i = 0; i < url.length; i++) {
        for (let j = 1; j <= 5; j++) {
            tasks.push((async () => {
                let fetch = new realIsomorphicFetch(crossFetch);
                let result = await fetch(url[i].replace(/xx/gi, '0'+j));
                let fileStream = fs.createWriteStream(fileName[i].replace(/xx/gi, '0'+j));
                result.body.pipe(fileStream);
                await new Promise((resolve => {
                    console.log(`${fileName[i].replace(/xx/gi, '0'+j)} Start Downloaded`);
                    fileStream.on('finish', () => {
                        console.log(`${fileName[i].replace(/xx/gi, '0'+j)} Downloaded`);
                        resolve();
                    });
                }));
            })());
        }
    }
    
    await Promise.all(tasks);
})();