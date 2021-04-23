const path = require('path');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const { VueLoaderPlugin } = require('vue-loader');
const WebpackDevServer = require('webpack-dev-server');

module.exports = (env, argv) => {
  // Fixes tailwind purging:
  process.env.NODE_ENV = argv.mode;

  const isProduction = argv.mode === 'production';

  let cssLoaders;
  if (isProduction) {
    cssLoaders = [MiniCssExtractPlugin.loader, "css-loader", "postcss-loader"]
  } else {
    cssLoaders = ["style-loader", "css-loader", "postcss-loader"]
  }

  const plugins = [
    new VueLoaderPlugin(),
    new HtmlWebpackPlugin({
      template: './src/index.html'
    }),
    // Vue feature flags (shuts up warnings), see: https://github.com/vuejs/vue-next/tree/master/packages/vue#bundler-build-feature-flags
    // Using defaults for now.
    new webpack.DefinePlugin({
      __VUE_OPTIONS_API__: JSON.stringify(true),
      __VUE_PROD_DEVTOOLS__: JSON.stringify(false)
    })
  ];

  if (argv.mode === 'production') {
    plugins.push(new MiniCssExtractPlugin())
  }

  return {
    mode: 'production',

    entry: './src/index.js',

    output: {
      path: path.resolve(__dirname, 'dist'),
      filename: 'todos.bundle.js',
    },

    plugins: plugins,

    module: {
      rules: [
        {
          test: /\.css$/i,
          use: cssLoaders
        },
        {
          test: /\.vue$/,
          loader: 'vue-loader'
        },
      ],
    },

    devServer: {
      contentBase: path.join(__dirname, 'dist'),
      compress: true,
      host: '0.0.0.0',
      port: 9000,
    },
  };
};
