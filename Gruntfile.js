module.exports = function(grunt) {
    grunt.initConfig({
        shell: {
            'node-pre-gyp-ia32': {
                command: 'node-pre-gyp configure build package --target_arch=ia32'
            },
            'node-pre-gyp-x64': {
                command: 'node-pre-gyp configure build package --target_arch=x64'
            },
            'upload-binaries': {
                command: 'node-pre-gyp-github publish --release --commitish main'
            }
        },
        copy: {
            ia32: {
                files: [
                    {src: 'build/Release/node-miniaudio.node', dest: 'lib/node-miniaudio_' + process.platform + '_ia32.node'},
                    {src: 'binding.js', dest: 'lib/binding.js'},
                    {src: 'index.js', dest: 'lib/index.js'}
                ]
            },
            x64: {
                files: [
                    {src: 'build/Release/node-miniaudio.node', dest: 'lib/node-miniaudio_' + process.platform + '_x64.node'},
                    {src: 'binding.js', dest: 'lib/binding.js'},
                    {src: 'index.js', dest: 'lib/index.js'}
                ]
            }
        }
    });

    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-shell');
    grunt.loadNpmTasks('grunt-contrib-copy');

    grunt.registerTask('build-pre-ia32', [
        'shell:node-pre-gyp-ia32',
        'copy:ia32'
    ]);

    grunt.registerTask('build-pre-x64', [
        'shell:node-pre-gyp-x64',
        'copy:x64'
    ]);

    grunt.registerTask('build-pre', [
        'build-pre-ia32',
        'build-pre-x64'
    ]);

    grunt.registerTask('upload', [
        'shell:upload-binaries'
    ]);

    grunt.registerTask('release', [
        'build-pre',
        'upload'
    ]);
};
