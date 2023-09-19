const { join, dirname, isAbsolute } = require('path')
const readFile = path => require('fs').readFileSync(path, { encoding: 'utf8' })
const exec = command => require('child_process').execSync(command, { encoding: 'utf8' })

const path = (p => isAbsolute(p) ? p : join(dirname(process.argv[1]), p))(process.argv[2])
const texSource = readFile(path).split('\\newline')

const output = texSource
  .map(line => exec(`latexmlmath "${line}"`) // sudo apt install -y latexml
      .replace(/\n/gm, '')
      .replace(/>\s*</gm, '><')
      .replace(/<\?xml.*<math.*display="block">/gm, '')
      .replace(/<\/math>$/gm, '')
      .replace(/<mo>⁢<\/mo>/gm, '')
      // .replace(/<mi[^>]*>(?<sequence>(\w(<\/mi><mi[^>]*>\w)*))<\/mi>/gm, function () {
      //     const sequence = arguments[arguments.length - 1].sequence
      //     return '<mi>' + sequence.replace(/<\/mi><mi[^>]*>/gm, '') + '</mi>'
      // })
      // .replace(/"/gm, `\\"`)
  )
  .join('</math><br><math>')

console.log(`<math>${output}</math>`)
