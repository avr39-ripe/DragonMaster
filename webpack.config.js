const path = require('path');
const TerserPlugin = require('terser-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CompressionPlugin = require('compression-webpack-plugin');

module.exports = {
    mode: 'development',

    resolve: {
        modules: [
            path.resolve(__dirname, "lib/application"),
            path.resolve(__dirname, "lib/binio"),
            path.resolve(__dirname, "lib/tempsensor"),
            path.resolve(__dirname, "files"),
            "node_modules"
        ]
    },

    entry: {
        index: './files/index.js',
        thermostat: './files/thermostat.js'
    },

    output: {
        path: path.join(__dirname, 'files'),
        filename: '[name].[contenthash:10].js'  // Using contenthash for caching
    },

    module: {
        rules: [
            {
                test: /\.(js|jsx)$/,
                exclude: /node_modules/,
                include: path.resolve(__dirname, 'src'),
                use: {
                    loader: 'babel-loader',
                    options: {
                        presets: [
                            [
                                '@babel/preset-env',
                                {
                                    targets: {
                                        browsers: ['last 2 versions']
                                    },
                                    modules: false  // Enables tree-shaking
                                }
                            ]
                        ],
                        plugins: [
                            // Additional Babel plugins can be added here.
                        ]
                    }
                }
            }
        ]
    },

    devtool: 'source-map',

    optimization: {
        minimize: true,
        minimizer: [
            new TerserPlugin({
                extractComments: false,
                terserOptions: {
                    compress: true,
                    mangle: true
                }
            })
        ]
    },

    plugins: [
        new HtmlWebpackPlugin({
            template: './files/index-template.html',
            inject: 'head',
            chunks: ['index']
        }),
        new HtmlWebpackPlugin({
            template: './files/thermostat-template.html',
            filename: path.resolve('./', './files/thermostat.html'),
            inject: 'body',
            chunks: ['thermostat']
        }),
        new CompressionPlugin({
            deleteOriginalAssets: true
        })
    ]
};
