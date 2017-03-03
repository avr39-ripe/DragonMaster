const path = require('path');
const webpack = require('webpack');
const BabiliPlugin = require("babili-webpack-plugin");

module.exports = {
	resolve: {
		modules: [path.resolve(__dirname, "lib/application"), path.resolve(__dirname, "lib/binio"), "node_modules"]
	},
	entry : './files/index_new.js',
	output : {
		filename : './files/bundle.js'
	},
	module : {
		loaders : [ {
			test : /\.js$/,
			exclude : /(node_modules|bower_components)/,
			loader : 'babel-loader',
			query : {
				presets : [ 'es2015' ]
			}
		} ]
	},
	devtool : 'source-map',
	plugins : [
	//    new webpack.optimize.UglifyJsPlugin({
	//      compress: { warnings: false }
	//    })
	new BabiliPlugin() ]
}
