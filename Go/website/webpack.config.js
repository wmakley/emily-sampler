const path = require('path');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const { VueLoaderPlugin } = require('vue-loader')

module.exports = (env, argv) => {
  // Fixes tailwind purging:
  process.env.NODE_ENV = argv.mode;

  let cssLoaders;
  if (argv.mode === 'production') {
    cssLoaders = [MiniCssExtractPlugin.loader, "css-loader", "postcss-loader"]
  } else {
    cssLoaders = ["style-loader", "css-loader", "postcss-loader"]
  }

  const plugins = [
    new VueLoaderPlugin()
  ];

  if (argv.mode === 'production') {
    plugins.push(new MiniCssExtractPlugin())
  }

  return {
    mode: 'production',

    entry: './src/main.js',

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
        }
      ],
    },

    devServer: {
      contentBase: path.join(__dirname, 'dist'),
      compress: true,
      port: 9000,
    },
  };
};
